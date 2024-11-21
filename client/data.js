import { initializeApp } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-app.js";
import { getFirestore, doc, setDoc, getDoc } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-firestore.js";

// Firebase 설정
const firebaseConfig = {
    apiKey: "AIzaSyDrwv8GrSacKk1B2yrJkz1oX7GUQhfWDh8",
    authDomain: "test-d13ce.firebaseapp.com",
    projectId: "test-d13ce",
    storageBucket: "test-d13ce.firebasestorage.app",
    messagingSenderId: "12340326841",
    appId: "1:12340326841:web:b72bf64fb4e5ff92690668",
    measurementId: "G-JQHF3XC6S0"
};

// Firebase 초기화
const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

// 사용자의 진행 상황 저장
export async function saveUserProgress(studentId, currentStage) {
    try {
        const userRef = doc(db, "userProgress", studentId);
        await setDoc(userRef, {
            lastStage: currentStage,
            lastUpdated: new Date().toISOString()
        }, { merge: true });
        console.log('진행 상황이 저장되었습니다.');
    } catch (error) {
        console.error('진행 상황 저장 중 오류 발생:', error);
    }
}

// 사용자의 진행 상황 불러오기
export async function loadUserProgress(studentId) {
    try {
        const userRef = doc(db, "userProgress", studentId);
        const docSnap = await getDoc(userRef);
        
        if (docSnap.exists()) {
            return docSnap.data().lastStage;
        } else {
            console.log('사용자 데이터가 없습니다. 첫 스테이지부터 시작합니다.');
            return 0;
        }
    } catch (error) {
        console.error('진행 상황 불러오기 중 오류 발생:', error);
        return 0;
    }
}
